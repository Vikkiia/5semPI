using ResultsCollection;
using System.Text.Json;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.IO;
using System.Threading;
namespace ResultsCollection
{
    public class ResultsService : IResultsService
    {
        private readonly string _filePath = "results.json";
        private readonly SemaphoreSlim _lock = new SemaphoreSlim(1, 1); 

        public ResultsService()
        {
           
            if (!File.Exists(_filePath))
            {
                File.WriteAllText(_filePath, "{}");
            }
        }

        private async Task<Dictionary<int, string>> LoadCollectionAsync()
        {
            
            var json = await File.ReadAllTextAsync(_filePath);
            
            var tempDict = JsonSerializer.Deserialize<Dictionary<int, string>>(json)
                           ?? new Dictionary<int, string>();

            // Преобразуем обратно в словарь int/string
            // (Этот блок нужен, если ключи в JSON сохраняются как строки)
            var dict = new Dictionary<int, string>();
            foreach (var kvp in tempDict)
            {
                if (int.TryParse(kvp.Key.ToString(), out int key))
                {
                    dict.Add(key, kvp.Value);
                }
            }
            return dict;
        }

        private async Task SaveCollectionAsync(Dictionary<int, string> dict)
        {
            var json = JsonSerializer.Serialize(dict, new JsonSerializerOptions { WriteIndented = true });
            await File.WriteAllTextAsync(_filePath, json);
        }

        

        public async Task<List<ResultItem>> GetAllAsync()
        {
            await _lock.WaitAsync();
            try
            {
                var dict = await LoadCollectionAsync();
                return dict.Select(kv => new ResultItem { Key = kv.Key, Value = kv.Value }).ToList();
            }
            finally
            {
                _lock.Release();
            }
        }

        
        public async Task<ResultItem?> GetAsync(int key)
        {
            await _lock.WaitAsync();
            try
            {
                var dict = await LoadCollectionAsync();
                if (dict.TryGetValue(key, out string? value))
                {
                    return new ResultItem { Key = key, Value = value };
                }
                return null;
            }
            finally
            {
                _lock.Release();
            }
        }

        public async Task<ResultItem> AddAsync(string value)
        {
            await _lock.WaitAsync();
            try
            {
                var dict = await LoadCollectionAsync();

                var newKey = dict.Keys.Any() ? dict.Keys.Max() + 1 : 1;

                dict.Add(newKey, value);
                await SaveCollectionAsync(dict);

                return new ResultItem { Key = newKey, Value = value };
            }
            finally
            {
                _lock.Release();
            }
        }

        public async Task<ResultItem?> UpdateAsync(int key, string value)
        {
            await _lock.WaitAsync();
            try
            {
                var dict = await LoadCollectionAsync();
                if (!dict.ContainsKey(key))
                {
                    return null;
                }
                dict[key] = value;
                await SaveCollectionAsync(dict);
                return new ResultItem { Key = key, Value = value };
            }
            finally
            {
                _lock.Release();
            }
        }

        
        public async Task<ResultItem?> DeleteAsync(int key)
        {
            await _lock.WaitAsync();
            try
            {
                var dict = await LoadCollectionAsync();
                if (!dict.TryGetValue(key, out string? value))
                {
                    return null;
                }

                
                dict.Remove(key);
                await SaveCollectionAsync(dict);

                return new ResultItem { Key = key, Value = value };
            }
            finally
            {
                _lock.Release();
            }
        }
    }
}