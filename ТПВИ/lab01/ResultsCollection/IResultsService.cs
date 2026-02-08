using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResultsCollection
{
    public interface IResultsService
    {
        Task<List<ResultItem>> GetAllAsync();
        Task<ResultItem?> GetAsync(int key);
        Task<ResultItem> AddAsync(string value);
        Task<ResultItem?> UpdateAsync(int key, string value);
        Task<ResultItem?> DeleteAsync(int key);
    }
}
