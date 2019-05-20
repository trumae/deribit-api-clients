/**
 * Deribit API
 * #Overview  Deribit provides three different interfaces to access the API:  * [JSON-RPC over Websocket](#json-rpc) * [JSON-RPC over HTTP](#json-rpc) * [FIX](#fix-api) (Financial Information eXchange)  With the API Console you can use and test the JSON-RPC API, both via HTTP and  via Websocket. To visit the API console, go to __Account > API tab >  API Console tab.__   ##Naming Deribit tradeable assets or instruments use the following system of naming:  |Kind|Examples|Template|Comments| |----|--------|--------|--------| |Future|<code>BTC-25MAR16</code>, <code>BTC-5AUG16</code>|<code>BTC-DMMMYY</code>|<code>BTC</code> is currency, <code>DMMMYY</code> is expiration date, <code>D</code> stands for day of month (1 or 2 digits), <code>MMM</code> - month (3 first letters in English), <code>YY</code> stands for year.| |Perpetual|<code>BTC-PERPETUAL</code>                        ||Perpetual contract for currency <code>BTC</code>.| |Option|<code>BTC-25MAR16-420-C</code>, <code>BTC-5AUG16-580-P</code>|<code>BTC-DMMMYY-STRIKE-K</code>|<code>STRIKE</code> is option strike price in USD. Template <code>K</code> is option kind: <code>C</code> for call options or <code>P</code> for put options.|   # JSON-RPC JSON-RPC is a light-weight remote procedure call (RPC) protocol. The  [JSON-RPC specification](https://www.jsonrpc.org/specification) defines the data structures that are used for the messages that are exchanged between client and server, as well as the rules around their processing. JSON-RPC uses JSON (RFC 4627) as data format.  JSON-RPC is transport agnostic: it does not specify which transport mechanism must be used. The Deribit API supports both Websocket (preferred) and HTTP (with limitations: subscriptions are not supported over HTTP).  ## Request messages > An example of a request message:  ```json {     \"jsonrpc\": \"2.0\",     \"id\": 8066,     \"method\": \"public/ticker\",     \"params\": {         \"instrument\": \"BTC-24AUG18-6500-P\"     } } ```  According to the JSON-RPC sepcification the requests must be JSON objects with the following fields.  |Name|Type|Description| |----|----|-----------| |jsonrpc|string|The version of the JSON-RPC spec: \"2.0\"| |id|integer or string|An identifier of the request. If it is included, then the response will contain the same identifier| |method|string|The method to be invoked| |params|object|The parameters values for the method. The field names must match with the expected parameter names. The parameters that are expected are described in the documentation for the methods, below.|  <aside class=\"warning\"> The JSON-RPC specification describes two features that are currently not supported by the API:  <ul> <li>Specification of parameter values by position</li> <li>Batch requests</li> </ul>  </aside>   ## Response messages > An example of a response message:  ```json {     \"jsonrpc\": \"2.0\",     \"id\": 5239,     \"testnet\": false,     \"result\": [         {             \"currency\": \"BTC\",             \"currencyLong\": \"Bitcoin\",             \"minConfirmation\": 2,             \"txFee\": 0.0006,             \"isActive\": true,             \"coinType\": \"BITCOIN\",             \"baseAddress\": null         }     ],     \"usIn\": 1535043730126248,     \"usOut\": 1535043730126250,     \"usDiff\": 2 } ```  The JSON-RPC API always responds with a JSON object with the following fields.   |Name|Type|Description| |----|----|-----------| |id|integer|This is the same id that was sent in the request.| |result|any|If successful, the result of the API call. The format for the result is described with each method.| |error|error object|Only present if there was an error invoking the method. The error object is described below.| |testnet|boolean|Indicates whether the API in use is actually the test API.  <code>false</code> for production server, <code>true</code> for test server.| |usIn|integer|The timestamp when the requests was received (microseconds since the Unix epoch)| |usOut|integer|The timestamp when the response was sent (microseconds since the Unix epoch)| |usDiff|integer|The number of microseconds that was spent handling the request|  <aside class=\"notice\"> The fields <code>testnet</code>, <code>usIn</code>, <code>usOut</code> and <code>usDiff</code> are not part of the JSON-RPC standard.  <p>In order not to clutter the examples they will generally be omitted from the example code.</p> </aside>  > An example of a response with an error:  ```json {     \"jsonrpc\": \"2.0\",     \"id\": 8163,     \"error\": {         \"code\": 11050,         \"message\": \"bad_request\"     },     \"testnet\": false,     \"usIn\": 1535037392434763,     \"usOut\": 1535037392448119,     \"usDiff\": 13356 } ``` In case of an error the response message will contain the error field, with as value an object with the following with the following fields:  |Name|Type|Description |----|----|-----------| |code|integer|A number that indicates the kind of error.| |message|string|A short description that indicates the kind of error.| |data|any|Additional data about the error. This field may be omitted.|  ## Notifications  > An example of a notification:  ```json {     \"jsonrpc\": \"2.0\",     \"method\": \"subscription\",     \"params\": {         \"channel\": \"deribit_price_index.btc_usd\",         \"data\": {             \"timestamp\": 1535098298227,             \"price\": 6521.17,             \"index_name\": \"btc_usd\"         }     } } ```  API users can subscribe to certain types of notifications. This means that they will receive JSON-RPC notification-messages from the server when certain events occur, such as changes to the index price or changes to the order book for a certain instrument.   The API methods [public/subscribe](#public-subscribe) and [private/subscribe](#private-subscribe) are used to set up a subscription. Since HTTP does not support the sending of messages from server to client, these methods are only availble when using the Websocket transport mechanism.  At the moment of subscription a \"channel\" must be specified. The channel determines the type of events that will be received.  See [Subscriptions](#subscriptions) for more details about the channels.  In accordance with the JSON-RPC specification, the format of a notification  is that of a request message without an <code>id</code> field. The value of the <code>method</code> field will always be <code>\"subscription\"</code>. The <code>params</code> field will always be an object with 2 members: <code>channel</code> and <code>data</code>. The value of the <code>channel</code> member is the name of the channel (a string). The value of the <code>data</code> member is an object that contains data  that is specific for the channel.   ## Authentication  > An example of a JSON request with token:  ```json {     \"id\": 5647,     \"method\": \"private/get_subaccounts\",     \"params\": {         \"access_token\": \"67SVutDoVZSzkUStHSuk51WntMNBJ5mh5DYZhwzpiqDF\"     } } ```  The API consists of `public` and `private` methods. The public methods do not require authentication. The private methods use OAuth 2.0 authentication. This means that a valid OAuth access token must be included in the request, which can get achived by calling method [public/auth](#public-auth).  When the token was assigned to the user, it should be passed along, with other request parameters, back to the server:  |Connection type|Access token placement |----|-----------| |**Websocket**|Inside request JSON parameters, as an `access_token` field| |**HTTP (REST)**|Header `Authorization: bearer ```Token``` ` value|  ### Additional authorization method - basic user credentials  <span style=\"color:red\"><b> ! Not recommended - however, it could be useful for quick testing API</b></span></br>  Every `private` method could be accessed by providing, inside HTTP `Authorization: Basic XXX` header, values with user `ClientId` and assigned `ClientSecret` (both values can be found on the API page on the Deribit website) encoded with `Base64`:  <code>Authorization: Basic BASE64(`ClientId` + `:` + `ClientSecret`)</code>   ### Additional authorization method - Deribit signature credentials  The Derbit service provides dedicated authorization method, which harness user generated signature to increase security level for passing request data. Generated value is passed inside `Authorization` header, coded as:  <code>Authorization: deri-hmac-sha256 id=```ClientId```,ts=```Timestamp```,sig=```Signature```,nonce=```Nonce```</code>  where:  |Deribit credential|Description |----|-----------| |*ClientId*|Can be found on the API page on the Deribit website| |*Timestamp*|Time when the request was generated - given as **miliseconds**. It's valid for **60 seconds** since generation, after that time any request with an old timestamp will be rejected.| |*Signature*|Value for signature calculated as described below | |*Nonce*|Single usage, user generated initialization vector for the server token|  The signature is generated by the following formula:  <code> Signature = HEX_STRING( HMAC-SHA256( ClientSecret, StringToSign ) );</code></br>  <code> StringToSign =  Timestamp + \"\\n\" + Nonce + \"\\n\" + RequestData;</code></br>  <code> RequestData =  UPPERCASE(HTTP_METHOD())  + \"\\n\" + URI() + \"\\n\" + RequestBody + \"\\n\";</code></br>   e.g. (using shell with ```openssl``` tool):  <code>&nbsp;&nbsp;&nbsp;&nbsp;ClientId=AAAAAAAAAAA</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;ClientSecret=ABCD</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Timestamp=$( date +%s000 )</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Nonce=$( cat /dev/urandom | tr -dc 'a-z0-9' | head -c8 )</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;URI=\"/api/v2/private/get_account_summary?currency=BTC\"</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;HttpMethod=GET</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Body=\"\"</code></br></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Signature=$( echo -ne \"${Timestamp}\\n${Nonce}\\n${HttpMethod}\\n${URI}\\n${Body}\\n\" | openssl sha256 -r -hmac \"$ClientSecret\" | cut -f1 -d' ' )</code></br></br> <code>&nbsp;&nbsp;&nbsp;&nbsp;echo $Signature</code></br></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;shell output> ea40d5e5e4fae235ab22b61da98121fbf4acdc06db03d632e23c66bcccb90d2c  (**WARNING**: Exact value depends on current timestamp and client credentials</code></br></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;curl -s -X ${HttpMethod} -H \"Authorization: deri-hmac-sha256 id=${ClientId},ts=${Timestamp},nonce=${Nonce},sig=${Signature}\" \"https://www.deribit.com${URI}\"</code></br></br>    ### Additional authorization method - signature credentials (WebSocket API)  When connecting through Websocket, user can request for authorization using ```client_credential``` method, which requires providing following parameters (as a part of JSON request):  |JSON parameter|Description |----|-----------| |*grant_type*|Must be **client_signature**| |*client_id*|Can be found on the API page on the Deribit website| |*timestamp*|Time when the request was generated - given as **miliseconds**. It's valid for **60 seconds** since generation, after that time any request with an old timestamp will be rejected.| |*signature*|Value for signature calculated as described below | |*nonce*|Single usage, user generated initialization vector for the server token| |*data*|**Optional** field, which contains any user specific value|  The signature is generated by the following formula:  <code> StringToSign =  Timestamp + \"\\n\" + Nonce + \"\\n\" + Data;</code></br>  <code> Signature = HEX_STRING( HMAC-SHA256( ClientSecret, StringToSign ) );</code></br>   e.g. (using shell with ```openssl``` tool):  <code>&nbsp;&nbsp;&nbsp;&nbsp;ClientId=AAAAAAAAAAA</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;ClientSecret=ABCD</code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Timestamp=$( date +%s000 ) # e.g. 1554883365000 </code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Nonce=$( cat /dev/urandom | tr -dc 'a-z0-9' | head -c8 ) # e.g. fdbmmz79 </code></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Data=\"\"</code></br></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;Signature=$( echo -ne \"${Timestamp}\\n${Nonce}\\n${Data}\\n\" | openssl sha256 -r -hmac \"$ClientSecret\" | cut -f1 -d' ' )</code></br></br> <code>&nbsp;&nbsp;&nbsp;&nbsp;echo $Signature</code></br></br>  <code>&nbsp;&nbsp;&nbsp;&nbsp;shell output> e20c9cd5639d41f8bbc88f4d699c4baf94a4f0ee320e9a116b72743c449eb994  (**WARNING**: Exact value depends on current timestamp and client credentials</code></br></br>   You can also check the signature value using some online tools like, e.g: [https://codebeautify.org/hmac-generator](https://codebeautify.org/hmac-generator) (but don't forget about adding *newline* after each part of the hashed text and remember that you **should use** it only with your **test credentials**).   Here's a sample JSON request created using the values from the example above:  <code> {                            </br> &nbsp;&nbsp;\"jsonrpc\" : \"2.0\",         </br> &nbsp;&nbsp;\"id\" : 9929,               </br> &nbsp;&nbsp;\"method\" : \"public/auth\",  </br> &nbsp;&nbsp;\"params\" :                 </br> &nbsp;&nbsp;{                        </br> &nbsp;&nbsp;&nbsp;&nbsp;\"grant_type\" : \"client_signature\",   </br> &nbsp;&nbsp;&nbsp;&nbsp;\"client_id\" : \"AAAAAAAAAAA\",         </br> &nbsp;&nbsp;&nbsp;&nbsp;\"timestamp\": \"1554883365000\",        </br> &nbsp;&nbsp;&nbsp;&nbsp;\"nonce\": \"fdbmmz79\",                 </br> &nbsp;&nbsp;&nbsp;&nbsp;\"data\": \"\",                          </br> &nbsp;&nbsp;&nbsp;&nbsp;\"signature\" : \"e20c9cd5639d41f8bbc88f4d699c4baf94a4f0ee320e9a116b72743c449eb994\"  </br> &nbsp;&nbsp;}                        </br> }                            </br> </code>   ### Access scope  When asking for `access token` user can provide the required access level (called `scope`) which defines what type of functionality he/she wants to use, and whether requests are only going to check for some data or also to update them.  Scopes are required and checked for `private` methods, so if you plan to use only `public` information you can stay with values assigned by default.  |Scope|Description |----|-----------| |*account:read*|Access to **account** methods - read only data| |*account:read_write*|Access to **account** methods - allows to manage account settings, add subaccounts, etc.| |*trade:read*|Access to **trade** methods - read only data| |*trade:read_write*|Access to **trade** methods - required to create and modify orders| |*wallet:read*|Access to **wallet** methods - read only data| |*wallet:read_write*|Access to **wallet** methods - allows to withdraw, generate new deposit address, etc.| |*wallet:none*, *account:none*, *trade:none*|Blocked access to specified functionality|    <span style=\"color:red\">**NOTICE:**</span> Depending on choosing an authentication method (```grant type```) some scopes could be narrowed by the server. e.g. when ```grant_type = client_credentials``` and ```scope = wallet:read_write``` it's modified by the server as ```scope = wallet:read```\"   ## JSON-RPC over websocket Websocket is the prefered transport mechanism for the JSON-RPC API, because it is faster and because it can support [subscriptions](#subscriptions) and [cancel on disconnect](#private-enable_cancel_on_disconnect). The code examples that can be found next to each of the methods show how websockets can be used from Python or Javascript/node.js.  ## JSON-RPC over HTTP Besides websockets it is also possible to use the API via HTTP. The code examples for 'shell' show how this can be done using curl. Note that subscriptions and cancel on disconnect are not supported via HTTP.  #Methods 
 *
 * OpenAPI spec version: 2.0.0
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.0.0-SNAPSHOT.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * PublicApi.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_API_PublicApi_H_
#define ORG_OPENAPITOOLS_CLIENT_API_PublicApi_H_


#include "../ApiClient.h"

#include "Object.h"
#include <cpprest/details/basic_types.h>
#include "../ModelBase.h"

#include <boost/optional.hpp>

namespace org {
namespace openapitools {
namespace client {
namespace api {

using namespace org::openapitools::client::model;



class  PublicApi 
{
public:

    explicit PublicApi( std::shared_ptr<ApiClient> apiClient );

    virtual ~PublicApi();

    /// <summary>
    /// Authenticate
    /// </summary>
    /// <remarks>
    /// Retrieve an Oauth access token, to be used for authentication of &#39;private&#39; requests.  Three methods of authentication are supported:  - &lt;code&gt;password&lt;/code&gt; - using email and and password as when logging on to the website - &lt;code&gt;client_credentials&lt;/code&gt; - using the access key and access secret that can be found on the API page on the website - &lt;code&gt;client_signature&lt;/code&gt; - using the access key that can be found on the API page on the website and user generated signature. The signature is calculated using some fields provided in the request, using formula described here [Deribit signature credentials](#additional-authorization-method-deribit-signature-credentials) - &lt;code&gt;refresh_token&lt;/code&gt; - using a refresh token that was received from an earlier invocation  The response will contain an access token, expiration period (number of seconds that the token is valid) and a refresh token that can be used to get a new set of tokens. 
    /// </remarks>
    /// <param name="grantType">Method of authentication</param>
    /// <param name="username">Required for grant type &#x60;password&#x60;</param>
    /// <param name="password">Required for grant type &#x60;password&#x60;</param>
    /// <param name="clientId">Required for grant type &#x60;client_credentials&#x60; and &#x60;client_signature&#x60;</param>
    /// <param name="clientSecret">Required for grant type &#x60;client_credentials&#x60;</param>
    /// <param name="refreshToken">Required for grant type &#x60;refresh_token&#x60;</param>
    /// <param name="timestamp">Required for grant type &#x60;client_signature&#x60;, provides time when request has been generated</param>
    /// <param name="signature">Required for grant type &#x60;client_signature&#x60;; it&#39;s a cryptographic signature calculated over provided fields using user **secret key**. The signature should be calculated as an HMAC (Hash-based Message Authentication Code) with &#x60;SHA256&#x60; hash algorithm</param>
    /// <param name="nonce">Optional for grant type &#x60;client_signature&#x60;; delivers user generated initialization vector for the server token (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="state">Will be passed back in the response (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="scope">Describes type of the access for assigned token, possible values: &#x60;connection&#x60;, &#x60;session&#x60;, &#x60;session:name&#x60;, &#x60;trade:[read, read_write, none]&#x60;, &#x60;wallet:[read, read_write, none]&#x60;, &#x60;account:[read, read_write, none]&#x60;, &#x60;expires:NUMBER&#x60; (token will expire after &#x60;NUMBER&#x60; of seconds).&lt;/BR&gt;&lt;/BR&gt; **NOTICE:** Depending on choosing an authentication method (&#x60;&#x60;&#x60;grant type&#x60;&#x60;&#x60;) some scopes could be narrowed by the server. e.g. when &#x60;&#x60;&#x60;grant_type &#x3D; client_credentials&#x60;&#x60;&#x60; and &#x60;&#x60;&#x60;scope &#x3D; wallet:read_write&#x60;&#x60;&#x60; it&#39;s modified by the server as &#x60;&#x60;&#x60;scope &#x3D; wallet:read&#x60;&#x60;&#x60; (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicAuthGet(
        utility::string_t grantType,
        utility::string_t username,
        utility::string_t password,
        utility::string_t clientId,
        utility::string_t clientSecret,
        utility::string_t refreshToken,
        utility::string_t timestamp,
        utility::string_t signature,
        boost::optional<utility::string_t> nonce,
        boost::optional<utility::string_t> state,
        boost::optional<utility::string_t> scope
    );
    /// <summary>
    /// Stop sending heartbeat messages.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    pplx::task<std::shared_ptr<Object>> publicDisableHeartbeatGet(
    );
    /// <summary>
    /// Retrieves announcements from the last 30 days.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    pplx::task<std::shared_ptr<Object>> publicGetAnnouncementsGet(
    );
    /// <summary>
    /// Retrieves the summary information such as open interest, 24h volume, etc. for all instruments for the currency (optionally filtered by kind).
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    /// <param name="kind">Instrument kind, if not provided instruments of all kinds are considered (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicGetBookSummaryByCurrencyGet(
        utility::string_t currency,
        boost::optional<utility::string_t> kind
    );
    /// <summary>
    /// Retrieves the summary information such as open interest, 24h volume, etc. for a specific instrument.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    pplx::task<std::shared_ptr<Object>> publicGetBookSummaryByInstrumentGet(
        utility::string_t instrumentName
    );
    /// <summary>
    /// Retrieves contract size of provided instrument.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    pplx::task<std::shared_ptr<Object>> publicGetContractSizeGet(
        utility::string_t instrumentName
    );
    /// <summary>
    /// Retrieves all cryptocurrencies supported by the API.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    pplx::task<std::shared_ptr<Object>> publicGetCurrenciesGet(
    );
    /// <summary>
    /// Retrieve the latest user trades that have occurred for PERPETUAL instruments in a specific currency symbol and within given time range.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    /// <param name="length">Specifies time period. &#x60;8h&#x60; - 8 hours, &#x60;24h&#x60; - 24 hours (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicGetFundingChartDataGet(
        utility::string_t instrumentName,
        boost::optional<utility::string_t> length
    );
    /// <summary>
    /// Provides information about historical volatility for given cryptocurrency.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    pplx::task<std::shared_ptr<Object>> publicGetHistoricalVolatilityGet(
        utility::string_t currency
    );
    /// <summary>
    /// Retrieves the current index price for the instruments, for the selected currency.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    pplx::task<std::shared_ptr<Object>> publicGetIndexGet(
        utility::string_t currency
    );
    /// <summary>
    /// Retrieves available trading instruments. This method can be used to see which instruments are available for trading, or which instruments have existed historically.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    /// <param name="kind">Instrument kind, if not provided instruments of all kinds are considered (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="expired">Set to true to show expired instruments instead of active ones. (optional, default to false)</param>
    pplx::task<std::shared_ptr<Object>> publicGetInstrumentsGet(
        utility::string_t currency,
        boost::optional<utility::string_t> kind,
        boost::optional<bool> expired
    );
    /// <summary>
    /// Retrieves historical settlement, delivery and bankruptcy events coming from all instruments within given currency.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    /// <param name="type">Settlement type (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="count">Number of requested items, default - &#x60;20&#x60; (optional, default to 0)</param>
    /// <param name="continuation">Continuation token for pagination (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="searchStartTimestamp">The latest timestamp to return result for (optional, default to 0)</param>
    pplx::task<std::shared_ptr<Object>> publicGetLastSettlementsByCurrencyGet(
        utility::string_t currency,
        boost::optional<utility::string_t> type,
        boost::optional<int32_t> count,
        boost::optional<utility::string_t> continuation,
        boost::optional<int32_t> searchStartTimestamp
    );
    /// <summary>
    /// Retrieves historical public settlement, delivery and bankruptcy events filtered by instrument name.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    /// <param name="type">Settlement type (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="count">Number of requested items, default - &#x60;20&#x60; (optional, default to 0)</param>
    /// <param name="continuation">Continuation token for pagination (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="searchStartTimestamp">The latest timestamp to return result for (optional, default to 0)</param>
    pplx::task<std::shared_ptr<Object>> publicGetLastSettlementsByInstrumentGet(
        utility::string_t instrumentName,
        boost::optional<utility::string_t> type,
        boost::optional<int32_t> count,
        boost::optional<utility::string_t> continuation,
        boost::optional<int32_t> searchStartTimestamp
    );
    /// <summary>
    /// Retrieve the latest trades that have occurred for instruments in a specific currency symbol and within given time range.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    /// <param name="startTimestamp">The earliest timestamp to return result for</param>
    /// <param name="endTimestamp">The most recent timestamp to return result for</param>
    /// <param name="kind">Instrument kind, if not provided instruments of all kinds are considered (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="count">Number of requested items, default - &#x60;10&#x60; (optional, default to 0)</param>
    /// <param name="includeOld">Include trades older than 7 days, default - &#x60;false&#x60; (optional, default to false)</param>
    /// <param name="sorting">Direction of results sorting (&#x60;default&#x60; value means no sorting, results will be returned in order in which they left the database) (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicGetLastTradesByCurrencyAndTimeGet(
        utility::string_t currency,
        int32_t startTimestamp,
        int32_t endTimestamp,
        boost::optional<utility::string_t> kind,
        boost::optional<int32_t> count,
        boost::optional<bool> includeOld,
        boost::optional<utility::string_t> sorting
    );
    /// <summary>
    /// Retrieve the latest trades that have occurred for instruments in a specific currency symbol.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="currency">The currency symbol</param>
    /// <param name="kind">Instrument kind, if not provided instruments of all kinds are considered (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="startId">The ID number of the first trade to be returned (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="endId">The ID number of the last trade to be returned (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="count">Number of requested items, default - &#x60;10&#x60; (optional, default to 0)</param>
    /// <param name="includeOld">Include trades older than 7 days, default - &#x60;false&#x60; (optional, default to false)</param>
    /// <param name="sorting">Direction of results sorting (&#x60;default&#x60; value means no sorting, results will be returned in order in which they left the database) (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicGetLastTradesByCurrencyGet(
        utility::string_t currency,
        boost::optional<utility::string_t> kind,
        boost::optional<utility::string_t> startId,
        boost::optional<utility::string_t> endId,
        boost::optional<int32_t> count,
        boost::optional<bool> includeOld,
        boost::optional<utility::string_t> sorting
    );
    /// <summary>
    /// Retrieve the latest trades that have occurred for a specific instrument and within given time range.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    /// <param name="startTimestamp">The earliest timestamp to return result for</param>
    /// <param name="endTimestamp">The most recent timestamp to return result for</param>
    /// <param name="count">Number of requested items, default - &#x60;10&#x60; (optional, default to 0)</param>
    /// <param name="includeOld">Include trades older than 7 days, default - &#x60;false&#x60; (optional, default to false)</param>
    /// <param name="sorting">Direction of results sorting (&#x60;default&#x60; value means no sorting, results will be returned in order in which they left the database) (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicGetLastTradesByInstrumentAndTimeGet(
        utility::string_t instrumentName,
        int32_t startTimestamp,
        int32_t endTimestamp,
        boost::optional<int32_t> count,
        boost::optional<bool> includeOld,
        boost::optional<utility::string_t> sorting
    );
    /// <summary>
    /// Retrieve the latest trades that have occurred for a specific instrument.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    /// <param name="startSeq">The sequence number of the first trade to be returned (optional, default to 0)</param>
    /// <param name="endSeq">The sequence number of the last trade to be returned (optional, default to 0)</param>
    /// <param name="count">Number of requested items, default - &#x60;10&#x60; (optional, default to 0)</param>
    /// <param name="includeOld">Include trades older than 7 days, default - &#x60;false&#x60; (optional, default to false)</param>
    /// <param name="sorting">Direction of results sorting (&#x60;default&#x60; value means no sorting, results will be returned in order in which they left the database) (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicGetLastTradesByInstrumentGet(
        utility::string_t instrumentName,
        boost::optional<int32_t> startSeq,
        boost::optional<int32_t> endSeq,
        boost::optional<int32_t> count,
        boost::optional<bool> includeOld,
        boost::optional<utility::string_t> sorting
    );
    /// <summary>
    /// Retrieves the order book, along with other market values for a given instrument.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">The instrument name for which to retrieve the order book, see [&#x60;getinstruments&#x60;](#getinstruments) to obtain instrument names.</param>
    /// <param name="depth">The number of entries to return for bids and asks. (optional, default to 0.0)</param>
    pplx::task<std::shared_ptr<Object>> publicGetOrderBookGet(
        utility::string_t instrumentName,
        boost::optional<double> depth
    );
    /// <summary>
    /// Retrieves the current time (in milliseconds). This API endpoint can be used to check the clock skew between your software and Deribit&#39;s systems.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    pplx::task<std::shared_ptr<Object>> publicGetTimeGet(
    );
    /// <summary>
    /// Retrieves aggregated 24h trade volumes for different instrument types and currencies.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    pplx::task<std::shared_ptr<Object>> publicGetTradeVolumesGet(
    );
    /// <summary>
    /// Publicly available market data used to generate a TradingView candle chart.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    /// <param name="startTimestamp">The earliest timestamp to return result for</param>
    /// <param name="endTimestamp">The most recent timestamp to return result for</param>
    pplx::task<std::shared_ptr<Object>> publicGetTradingviewChartDataGet(
        utility::string_t instrumentName,
        int32_t startTimestamp,
        int32_t endTimestamp
    );
    /// <summary>
    /// Method used to introduce the client software connected to Deribit platform over websocket. Provided data may have an impact on the maintained connection and will be collected for internal statistical purposes. In response, Deribit will also introduce itself.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="clientName">Client software name</param>
    /// <param name="clientVersion">Client software version</param>
    pplx::task<std::shared_ptr<Object>> publicHelloGet(
        utility::string_t clientName,
        utility::string_t clientVersion
    );
    /// <summary>
    /// Signals the Websocket connection to send and request heartbeats. Heartbeats can be used to detect stale connections. When heartbeats have been set up, the API server will send &#x60;heartbeat&#x60; messages and &#x60;test_request&#x60; messages. Your software should respond to &#x60;test_request&#x60; messages by sending a &#x60;/api/v2/public/test&#x60; request. If your software fails to do so, the API server will immediately close the connection. If your account is configured to cancel on disconnect, any orders opened over the connection will be cancelled.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="interval">The heartbeat interval in seconds, but not less than 10</param>
    pplx::task<std::shared_ptr<Object>> publicSetHeartbeatGet(
        double interval
    );
    /// <summary>
    /// Subscribe to one or more channels.
    /// </summary>
    /// <remarks>
    /// Subscribe to one or more channels.  This is the same method as [/private/subscribe](#private_subscribe), but it can only be used for &#39;public&#39; channels. 
    /// </remarks>
    /// <param name="channels">A list of channels to subscribe to.</param>
    pplx::task<std::shared_ptr<Object>> publicSubscribeGet(
        std::vector<utility::string_t> channels
    );
    /// <summary>
    /// Tests the connection to the API server, and returns its version. You can use this to make sure the API is reachable, and matches the expected version.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="expectedResult">The value \&quot;exception\&quot; will trigger an error response. This may be useful for testing wrapper libraries. (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicTestGet(
        boost::optional<utility::string_t> expectedResult
    );
    /// <summary>
    /// Get ticker for an instrument.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instrumentName">Instrument name</param>
    pplx::task<std::shared_ptr<Object>> publicTickerGet(
        utility::string_t instrumentName
    );
    /// <summary>
    /// Method used to introduce the client software connected to Deribit platform over websocket. Provided data may have an impact on the maintained connection and will be collected for internal statistical purposes. In response, Deribit will also introduce itself.
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="field">Name of the field to be validated, examples: postal_code, date_of_birth</param>
    /// <param name="value">Value to be checked</param>
    /// <param name="value2">Additional value to be compared with (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    pplx::task<std::shared_ptr<Object>> publicValidateFieldGet(
        utility::string_t field,
        utility::string_t value,
        boost::optional<utility::string_t> value2
    );

protected:
    std::shared_ptr<ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_API_PublicApi_H_ */
