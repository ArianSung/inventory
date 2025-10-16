# Gemini AI Integration Features

This document describes the Google Gemini AI integration features added to the Inventory Manager application.

## Overview

The Inventory Manager now includes AI-powered features using Google's Gemini API to enhance productivity and provide intelligent insights.

## Features

### 1. AI Settings Configuration

**Location:** Settings Tab (설정 탭)

**Features:**
- Configure Gemini API Key
- Enable/Disable Gemini AI features
- Test API connection

**How to Use:**
1. Navigate to the Settings tab
2. Enter your Google Gemini API key in the "API Key" field
3. Check the "Gemini AI 활성화" checkbox to enable AI features
4. Click "연결 테스트" to verify the connection

**Getting an API Key:**
1. Visit [Google AI Studio](https://makersuite.google.com/app/apikey)
2. Create a new API key
3. Copy and paste it into the application

### 2. AI Product Description Generator

**Location:** Add Product Dialog (품목 추가 대화상자)

**Features:**
- Automatically generates professional product descriptions
- Uses product name, brand, and category information
- Generates descriptions in Korean
- Editable output for customization

**How to Use:**
1. Open the Add Product dialog (품목 추가)
2. Fill in the product name, brand, and category
3. Click the "AI 설명 생성" button
4. Review and edit the generated description as needed
5. Save the product

**Benefits:**
- Saves time writing product descriptions
- Ensures consistent, professional descriptions
- Helps with marketing and product presentation

### 3. AI Inventory Insights

**Location:** Main Inventory View (재고 현황 탭)

**Features:**
- Analyzes low-stock inventory items
- Provides reorder recommendations
- Suggests priority actions
- Identifies trends and patterns

**How to Use:**
1. Navigate to the Inventory tab (재고 현황)
2. Click the "AI 인사이트" button
3. Review the AI-generated insights in the dialog box
4. Insights are also logged in the activity log

**Benefits:**
- Proactive inventory management
- Data-driven decision making
- Automatic identification of critical stock issues
- Time-saving analysis

## Technical Details

### GeminiManager Class

The `GeminiManager` class is implemented as a singleton and provides:
- HTTP API communication with Google Gemini
- JSON request/response handling
- Error management
- Multiple AI functions:
  - `GenerateProductDescription()` - Product description generation
  - `GetInventoryInsight()` - Inventory analysis
  - `SuggestReorderQuantity()` - Reorder suggestions
  - `AnalyzeCategory()` - Category analysis

### Configuration

Gemini settings are stored in the application configuration and persist between sessions. The configuration includes:
- API Key (encrypted in production)
- Model selection (default: gemini-pro)
- Enable/disable state

### API Usage

The application uses the Gemini REST API:
- Endpoint: `https://generativelanguage.googleapis.com/v1beta/models/{model}:generateContent`
- Method: POST
- Content-Type: application/json

### Error Handling

The integration includes comprehensive error handling:
- API connection failures
- Invalid API keys
- Rate limiting
- Timeout handling
- User-friendly error messages in Korean

## Security Considerations

1. **API Key Storage**: Store your API key securely
2. **Access Control**: Limit who can access the settings
3. **Network Security**: Use HTTPS for all API communication
4. **Rate Limiting**: Be aware of API usage limits

## Troubleshooting

### "Gemini AI 기능이 활성화되지 않았습니다"
- Ensure you've entered a valid API key in Settings
- Check the "Gemini AI 활성화" checkbox
- Test the connection using "연결 테스트"

### "API 연결 실패"
- Verify your internet connection
- Check that the API key is valid
- Ensure you haven't exceeded API rate limits
- Check firewall settings

### Empty or Invalid Responses
- Check the activity log for detailed error messages
- Verify the API key has appropriate permissions
- Try testing with a simple request first

## Future Enhancements

Potential future features:
- Smart search with natural language queries
- Automated category suggestions
- Predictive inventory forecasting
- Batch description generation
- Multi-language support
- Custom AI prompts and templates

## API Costs

Google Gemini API usage may incur costs depending on your usage tier. Please refer to [Google's pricing documentation](https://ai.google.dev/pricing) for current rates.

## Support

For issues or questions:
1. Check the application logs for error details
2. Verify your Gemini API configuration
3. Consult Google's Gemini API documentation
4. Contact the development team

---

**Version:** 1.0  
**Last Updated:** 2025-10-16  
**Model Used:** gemini-pro
